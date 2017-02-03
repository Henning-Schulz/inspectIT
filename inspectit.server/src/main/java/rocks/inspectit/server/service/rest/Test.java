/**
 *
 */
package rocks.inspectit.server.service.rest;

import java.util.ArrayList;
import java.util.List;

import org.codehaus.jackson.annotate.JsonSubTypes;
import org.codehaus.jackson.annotate.JsonTypeInfo;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/my")
public class Test {
	@RequestMapping(method = RequestMethod.GET, value = "test")
	@ResponseBody
	public String test(@RequestParam(value = "id", required = true) int value) {
		System.out.println("Id is " + value);
		return "My Id was:" + value;
	}

	@RequestMapping(method = RequestMethod.GET, value = "bla")
	@ResponseBody
	public ClassA bla(@RequestParam(value = "aval", required = true) ClassA a) {
		System.out.println("Class of a is " + a.getClass().getName());
		return a;
	}

	@RequestMapping(method = RequestMethod.POST, value = "postTest")
	@ResponseBody
	public String postTest(@RequestParam(value = "val", required = true) String value) {
		return "Value was: " + value;
	}

	@RequestMapping(method = RequestMethod.POST, value = "another")
	@ResponseBody
	public List<Object> anotherTest(@RequestBody String[] values) {
		List<Object> result = new ArrayList<>();
		result.add(values[0]);
		result.add("Additional value");
		ClassA a = new ClassA();
		a.setName("aName");
		a.setId(42);
		result.add(a);
		return result;
	}

	@RequestMapping(method = RequestMethod.POST, value = "yetAnother")
	@ResponseBody
	public List<Object> yetAnotherTest(@RequestBody ClassA a, @RequestParam(value = "string") String string, @RequestParam(value = "long") long l) {
		List<Object> result = new ArrayList<>();
		result.add("Additional value");
		result.add(a);
		result.add(string);
		result.add(l);
		result.add("Type of a: " + a.getClass().getName());
		return result;
	}

	@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.PROPERTY, property = "objectType")
	@JsonSubTypes({ @JsonSubTypes.Type(name = "ClassB", value = ClassB.class), @JsonSubTypes.Type(name = "ClassA", value = ClassA.class) })
	public static class ClassA {
		String name;
		long id;

		public ClassA() {

		}

		/**
		 * Gets {@link #name}.
		 *
		 * @return {@link #name}
		 */
		public String getName() {
			return name;
		}

		/**
		 * Sets {@link #name}.
		 *
		 * @param name
		 *            New value for {@link #name}
		 */
		public void setName(String name) {
			this.name = name;
		}

		/**
		 * Gets {@link #id}.
		 *
		 * @return {@link #id}
		 */
		public long getId() {
			return id;
		}

		/**
		 * Sets {@link #id}.
		 *
		 * @param id
		 *            New value for {@link #id}
		 */
		public void setId(long id) {
			this.id = id;
		}
	}

	public static class ClassB extends ClassA {
		int intVal;

		/**
		 * Sets {@link #intVal}.
		 *
		 * @param intVal
		 *            New value for {@link #intVal}
		 */
		public void setIntVal(int intVal) {
			this.intVal = intVal;
		}

		/**
		 * Gets {@link #intVal}.
		 *
		 * @return {@link #intVal}
		 */
		public int getIntVal() {
			return intVal;
		}
	}

}
